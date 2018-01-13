package twitter

import (
    "encoding/json"
    "fmt"
    //"github.com/kurrik/oauth1a"
    "github.com/kurrik/twittergo"
    "io/ioutil"
)

type TwitterFeeder struct {
    ScreenName string
}

func GetJsonFromFile(fileName string) map[string]interface{} {
    jsonBytes, err := ioutil.ReadFile(fileName)
    if err != nil {
        panic(fmt.Sprintf("Couldn't read file [%s] :: %s", fileName, err.Error()))
    }
    var jsonObj map[string]interface{}
    err = json.Unmarshal(jsonBytes, &jsonObj)
    if err != nil {
        panic(fmt.Sprintf("Couldnt interpret bytes as json [%s] :: %s", jsonBytes, err.Error()))
    }
    return jsonObj
}

func LoadCredentials(credentialsFile string) *twittergo.Client {
    if credentialsFile == "dummy" {
        return nil
    } else {
        return nil
    }
}

func NewTwitterFeeder(feedCredentials, feedConfig string) *TwitterFeeder {
    tf := new(TwitterFeeder)

    configAsJson := GetJsonFromFile(feedConfig)
    tf.ScreenName = configAsJson["ScreenName"].(string)

    credentialsJson := GetJsonFromFile(feedCredentials)
    credentials := LoadCredentials(feedCredentials)
    fmt.Printf("Credentials... \nkey: %s, secret: %s\ncredentials obj: %s\n....End Credentials\n", credentialsJson["Key"].(string), credentialsJson["Secret"].(string), credentials)

    return tf
}

func (tf *TwitterFeeder) GetFeed () []string {
    return []string{tf.ScreenName}
}
