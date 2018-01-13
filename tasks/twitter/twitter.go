package twitter

import (
    "encoding/json"
    "fmt"
    "io/ioutil"
)

type TwitterFeeder struct {
    ScreenName string
}

func NewTwitterFeeder (feedConfig string) *TwitterFeeder {
    tf := new(TwitterFeeder)
    configBytes, err := ioutil.ReadFile(feedConfig)
    if err != nil {
        panic(fmt.Sprintf("Couldn't read file [%s] :: %s", feedConfig, err.Error()))
    }
    var configAsJson map[string]interface{}
    err = json.Unmarshal(configBytes, &configAsJson)
    if err != nil {
        panic(fmt.Sprintf("Couldnt interpret bytes as json [%s] :: %s", configBytes, err.Error()))
    }
    tf.ScreenName = configAsJson["ScreenName"].(string)
    return tf
}

func (tf *TwitterFeeder) GetFeed () []string {
    return []string{tf.ScreenName}
}
