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
        fmt.Printf("Starting reader %d...\n", c)
        for {
            // DO STUFF
            break;
        }
        fmt.Printf("Ending reader %d...\n", c)
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
