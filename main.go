package main

import (
    "fmt"
    "rsserver/tasks/twitter"
)

func main() {
    fmt.Print("Hello, world!")
    fmt.Print(twitter.GetFeed())
}
