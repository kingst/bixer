import com.twitter.finagle.http
import com.twitter.concurrent.exp.AsyncStream
import com.twitter.conversions.time._
import com.twitter.finagle.http.{Request, Response, Status}
import com.twitter.finagle.{Http, Service}
import com.twitter.io.{Buf, Reader, Writer}
import com.twitter.util.{Await, Future, Base64StringEncoder}

import java.util.concurrent.LinkedBlockingQueue

import sys.process._

object HttpStreamingServer extends App {

  val queue = new LinkedBlockingQueue[String]()

  def video(): AsyncStream[String] = AsyncStream.fromFuture(Future.value(queue.take)) ++ video

  val proc = Process("./test.sh")

  val pio = new ProcessIO(_ => (),
    stdout => scala.io.Source.fromInputStream(stdout).getLines.foreach { line =>
      if (queue.size < 1000) queue.put(line)
    },
    _ => ())

  val service = new Service[Request, Response] {

    def apply(request: Request) = {
      proc.run(pio)
      @volatile var messages: AsyncStream[Buf] = video().map(n => Buf.ByteArray(Base64StringEncoder.decode(n)))
      val writable = Reader.writable()
      // Start writing thread.
      messages.foreachF(writable.write)
      val response = Response(request.version, Status.Ok, writable)
      response.setContentType("video/mp2t")
      Future.value(response)
    }
  }

  Await.result(Http.server
    // Translate buffered writes into HTTP chunks.
    .withStreaming(enabled = true)
    // Listen on port 8080.
    .serve("0.0.0.0:8080", service)
  )
}
