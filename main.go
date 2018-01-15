package main

import (
    "fmt"
    "rsserver/tasks/twitter"
)

func main() {
    feed := twitter.NewTwitterFeeder("tasks/twitter/credentials.secret.json", "tasks/twitter/config.json")
    fmt.Println(feed.GetFeed(1))
}
