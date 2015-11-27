name := "quickstart"

version := "1.0"

scalaVersion := "2.10.6"

libraryDependencies += "com.twitter" %% "finagle-http" % "6.30.0"

mainClass in assembly := Some("HttpStreamingServer")
