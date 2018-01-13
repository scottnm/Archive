package main

import (
    "fmt"
    "rsserver/tasks/twitter"
)

func main() {
    feed := twitter.NewTwitterFeeder("tasks/twitter/config.json")
    fmt.Println(feed.GetFeed())
}
