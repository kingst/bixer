#!/usr/bin/env python
import json
import re
import sys
import xmltodict

channel_map = {}

prog = re.compile('(\d+)\s')

def parse_channel_id(path):
    for item in path:
        if item[0] == "channel":
            return item[1]["id"]

def handle_channel(path, item):
    channel_id = parse_channel_id(path)
    if channel_id:
        result = prog.match(item["display-name"][0])
        if result:
            channel_map[result.group(1)] = channel_id

    return True

xmltodict.parse(sys.stdin, item_depth=2, item_callback=handle_channel)

print(json.dumps(channel_map, indent=4))


