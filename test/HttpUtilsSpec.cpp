#include <vector>
#include <map>
#include <string>

#include "HttpUtils.h"
#include "MockSocket.h"

#include "gtest/gtest.h"

using namespace std;

TEST(HttpUtils, Split) {
  vector<string> result = HttpUtils::split("./a b   c ", ' ');
  vector<string> parsed;
  parsed.push_back("./a");
  parsed.push_back("b");
  parsed.push_back("c");
  EXPECT_EQ(result, parsed);
}

TEST(HttpUtils, SplitAllSpaces) {
  vector<string> result = HttpUtils::split("       ", ' ');
  vector<string> parsed;
  EXPECT_EQ(result, parsed);
}

TEST(HttpUtils, SplitNoSpaces) {
  vector<string> result = HttpUtils::split("asdf", ' ');
  vector<string> parsed;
  parsed.push_back("asdf");
  EXPECT_EQ(result, parsed);
}

TEST(HttpUtils, Params) {
  map<string, string> result = HttpUtils::params("channel=1&program=512");
  map<string, string> parsed;
  parsed["channel"] = "1";
  parsed["program"] = "512";
  EXPECT_EQ(result, parsed);
}

TEST(HttpUtils, OneParam) {
  map<string, string> result = HttpUtils::params("program=512");
  map<string, string> parsed;
  parsed["program"] = "512";
  EXPECT_EQ(result, parsed);
}

TEST(HttpUtils, ParamsExceptions) {
  ASSERT_THROW(HttpUtils::params("asdf"), MalformedQueryString);
  ASSERT_THROW(HttpUtils::params("asdf=1=a"), MalformedQueryString);
}

TEST(HttpUtils, ChunkedEncoding) {
  FakeSocket socket;

  HttpUtils::writeChunk(&socket, "hello", 5);
  HttpUtils::writeLastChunk(&socket);
  EXPECT_EQ(string("5\r\nhello\r\n0\r\n\r\n"), socket.output);
}

TEST(HttpUtils, ChunkedEncoding2) {
  FakeSocket socket;

  HttpUtils::writeChunk(&socket, "hello", 5);
  HttpUtils::writeChunk(&socket, "hellohello", 10);
  HttpUtils::writeLastChunk(&socket);
  EXPECT_EQ(string("5\r\nhello\r\na\r\nhellohello\r\n0\r\n\r\n"),
	    socket.output);
}
