package main

import (
    "fmt"
    "rsserver/tasks/twitter"
)

func main() {
    feed := twitter.NewTwitterFeeder("scottnmunro")
    fmt.Println(feed.GetFeed())
}
