#!/usr/bin/env python
import json
import re
import sys

channel_map = json.load(sys.stdin)

scan_file = open(sys.argv[1])
channel_prog = re.compile('SCANNING[:]\s(\d+)')
program_prog = re.compile('PROGRAM\s(\d+)[:]\s+(\d+)\s+(\S+)\s+')

channel = None
program = None

print("#EXTM3U")
for line in scan_file.readlines():
    result = channel_prog.match(line)
    if result:
        channel = result.group(1)

    result = program_prog.match(line)
    if result:
        program = result.group(1)
        channel_number = result.group(2)
        if channel_number in channel_map:
            channel_name = channel_number + " " + result.group(3)
            channel_id = channel_map[channel_number]
            print('#EXTINF:-1 tvg-id="' + channel_id + '", ' + channel_name)
            print("http://localhost:8080/stream.ts?channel=" + channel +
                  "&program=" + program);
