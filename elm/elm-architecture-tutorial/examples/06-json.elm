module Main exposing (Model, Msg(..), getRandomCatGif, gifDecoder, init, main, subscriptions, update, view, viewGif)

import Browser
import Html exposing (..)
import Html.Attributes exposing (..)
import Html.Events exposing (..)
import Http
import Json.Decode exposing (Decoder, field, map2, string)



-- MAIN


main =
    Browser.element
        { init = init
        , update = update
        , subscriptions = subscriptions
        , view = view
        }



-- MODEL


type alias Cat =
    { url : String
    , title : String
    }


type HttpFailure
    = FailureUrl String
    | Timeout
    | NetworkError
    | BadStatus Int
    | BadBody String


type Interaction
    = Failure HttpFailure
    | Loading
    | Success Cat



-- type Interaction
--     = HttpFailure
--     | Loading
--     | Success Cat


type alias Model =
    { counter : Int
    , inter : Interaction
    , cat : Cat
    }


noneCat : Cat
noneCat =
    { title = "None", url = "" }


init : () -> ( Model, Cmd Msg )
init _ =
    ( { counter = 0, inter = Loading, cat = noneCat }, getRandomCatGif )



-- UPDATE


type Msg
    = MorePlease
    | TryAgain
    | GotGif (Result Http.Error Cat)


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        MorePlease ->
            ( { model | counter = 0, inter = Loading }, getRandomCatGif )

        TryAgain ->
            ( { model | inter = Loading }, getRandomCatGif )

        GotGif result ->
            case result of
                Ok cat ->
                    ( { model | inter = Success cat }, Cmd.none )

                Err errorType ->
                    case errorType of
                        Http.BadUrl url ->
                            ( { model | counter = model.counter + 1, inter = Failure (FailureUrl url) }
                            , Cmd.none
                            )

                        Http.Timeout ->
                            ( { model | counter = model.counter + 1, inter = Failure Timeout }
                            , Cmd.none
                            )

                        -- The only error message which launches another get image
                        -- automatically
                        Http.NetworkError ->
                            ( { model | counter = model.counter + 1, inter = Failure NetworkError }
                              , getRandomCatGif
                            )

                        Http.BadStatus errorIdx ->
                            ( { model | counter = model.counter + 1, inter = Failure (BadStatus errorIdx) }
                            , Cmd.none
                            )

                        Http.BadBody body ->
                            ( { model | counter = model.counter + 1, inter = Failure (BadBody body) }
                            , Cmd.none
                            )



-- SUBSCRIPTIONS


subscriptions : Model -> Sub Msg
subscriptions model =
    Sub.none



-- VIEW


view : Model -> Html Msg
view model =
    div []
        [ h2 [] [ text "Random Cats" ]
        , viewContent model
        ]


httpFailureToString : HttpFailure -> String
httpFailureToString failure =
    case failure of
        FailureUrl url ->
            "Failure Url " ++ url

        Timeout ->
            "Timeout "

        NetworkError ->
            "Network Error "

        BadStatus errorIdx ->
            "Bad Status " ++ String.fromInt errorIdx

        BadBody body ->
            "Bad Body " ++ body


viewContent : Model -> Html Msg
viewContent model =
    case model.inter of
        Loading ->
            div []
                [ viewError model.counter
                , text "Loading..."
                , viewGif model.cat
                ]

        Success cat ->
            div []
                [ viewError model.counter
                , button [ onClick MorePlease, style "display" "block" ] [ text "More Please!" ]
                , viewGif cat
                ]

        Failure failure ->
            viewFailure model failure


viewFailure : Model -> HttpFailure -> Html Msg
viewFailure model failure =
    div []
        [ viewError model.counter
        , text (httpFailureToString failure)
        , button [ onClick TryAgain ] [ text "Try Again!" ]
        , viewGif model.cat
        ]


viewError : Int -> Html Msg
viewError counter =
    div []
        [ text ("Nb error: " ++ String.fromInt counter) ]


viewGif : Cat -> Html Msg
viewGif cat =
    div [ style "text-align" "center" ]
        [ h3 [] [ text cat.title ]
        , img [ src cat.url ] []
        ]



-- HTTP


getRandomCatGif : Cmd Msg
getRandomCatGif =
    Http.get
        { url = "https://api.giphy.com/v1/gifs/random?api_key=dc6zaTOxFJmzC&tag=cat"
        , expect = Http.expectJson GotGif gifDecoder
        }


gifDecoder : Decoder Cat
gifDecoder =
    map2 Cat
        (field "data" (field "image_url" string))
        (field "data" (field "title" string))
