from static_vars import static_vars
from enum import Enum
from collections import defaultdict
import re

def main():
    with open("data.txt", "r", encoding="utf8") as data:
        events = [parse_event(line.rstrip()) for line in data.readlines()]
        events = sorted(events, key=lambda x: "{} {}".format(x[0], x[1]))
        guards = make_sleep_patterns(events)
        sleepy_guard = find_sleepy_guard(guards)
        sleepy_minute = find_sleepy_minute(guards[sleepy_guard])
        print("Guard #{} slept the most on minute {}".format(sleepy_guard, sleepy_minute))
        print("  {} * {} = {}".format(sleepy_guard, sleepy_minute, sleepy_guard*sleepy_minute))
        guard, minute = find_sleepiest_on_same_minute(guards)
        print("Guard #{} slept the most on minute {}".format(guard, minute))
        print("  {} * {} = {}".format(guard, minute, guard*minute))

def make_sleep_patterns(events):
    guards = defaultdict(make_minutetable)
    on_guard = None
    start_sleep = None
    for date, time, event in events:
        if event == "falls asleep":
            start_sleep = parse_minute(time)
        elif event == "wakes up":
            end_sleep = parse_minute(time)
            fill_sleep(guards[on_guard], start_sleep, end_sleep)
            start_sleep = None
        else:
            on_guard = parse_guard_id(event)
    return guards

def find_sleepy_guard(guards):
    totals = [(guard_id, sum(minutes)) for guard_id, minutes in guards.items()]
    return max(totals, key=lambda x: x[1])[0]

def find_sleepy_minute(minutes):
    indexed_minutes = enumerate(minutes)
    return max(indexed_minutes, key=lambda x: x[1])[0]

def find_sleepiest_on_same_minute(guards):
    sleepiest_minute = [
        (guard_id, find_sleepy_minute(minutes))
        for guard_id, minutes in guards.items()
    ]
    return max(sleepiest_minute, key=lambda x: guards[x[0]][x[1]])

def parse_minute(time):
    return int(time.split(':')[1])

def fill_sleep(minutetable, start, stop):
    for minute in range(start, stop):
        minutetable[minute] += 1

def make_minutetable():
    return [0 for _ in range(60)]

@static_vars(matcher = re.compile(r'^Guard #(\d+) begins shift$'))
def parse_guard_id(event):
    m = parse_guard_id.matcher.match(event)
    if not m:
        raise Exception("Invalid event: {}".format(event))
    return int(m.group(1))

@static_vars(matcher = re.compile(r'^\[(\d+-\d+-\d+) (\d+:\d+)\] (.*)$'))
def parse_event(event):
    m = parse_event.matcher.match(event)
    if not m:
        raise Exception("Invalid data: {}".format(event))
    return (m.group(1), m.group(2), m.group(3))

if __name__ == '__main__':
    main()
