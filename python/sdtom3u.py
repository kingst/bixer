#!/usr/bin/env python
import json
import sys
import xmltodict

channel_map = {}

def parse_channel_id(path):
    for item in path:
        if item[0] == "channel":
            return item[1]["id"]

def handle_channel(path, item):
    channel_id = parse_channel_id(path)
    if channel_id:
        channel_map[item["display-name"][0]] = channel_id

    return True

xmltodict.parse(sys.stdin, item_depth=2, item_callback=handle_channel)

print(json.dumps(channel_map, indent=4))


