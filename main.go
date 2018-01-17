package main

import (
    "fmt"
    //"rsserver/tasks/twitter"
    "rsserver/feeds/stub"
)

func main() {
    //feed := twitter.NewTwitterFeeder("tasks/twitter/credentials.secret.json", "tasks/twitter/config.json")
    feed := stub.NewStubFeedReader()
    fmt.Println(feed.ReadFeed(1))
}
