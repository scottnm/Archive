package main

import (
    "fmt"
    "sync"
    //"rsserver/tasks/twitter"
    "rsserver/feeds"
    "rsserver/feeds/stub"
)

func StartFeedReader(wg *sync.WaitGroup, feedReader feeds.FeedReader, c uint8) {
    wg.Add(1)
    go func(wg *sync.WaitGroup, feedReader feeds.FeedReader) {
        for i := uint8(0); i < c; i++ {
            feedReader.SleepUntilScheduled()
            feedItems := feedReader.ReadFeed()
            for _, item := range feedItems {
                fmt.Printf("%d> Title: %v\tLink: %v\tDataAsHtml: %v\n", c, item.Title(), item.Link(), item.DataAsHtml())
            }
        }
        wg.Done()
    }(wg, feedReader)
}

func main() {
    //feed := twitter.NewTwitterFeeder("tasks/twitter/credentials.secret.json", "tasks/twitter/config.json")
    wg := new(sync.WaitGroup)
    for i := uint8(1); i <= 3; i++ {
        StartFeedReader(wg, stub.NewStubFeedReader(i, i * 3), i)
    }
    fmt.Println("Waiting...")
    wg.Wait()
    fmt.Println("Done.")
}
