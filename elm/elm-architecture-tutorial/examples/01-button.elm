module Main exposing (Model, Msg(..), init, main, update, view)

import Browser
import Html exposing (Html, button, div, text)
import Html.Events exposing (onClick)
import Debug


main =
    Browser.sandbox { init = init, update = update, view = view }



-- MODEL


type alias Model =
    Int


init : Model
init =
    2 ^ 1023 + 2 ^ 1022



-- UPDATE


type Msg
    = Increment
    | Decrement
    | Reset


update : Msg -> Model -> Model
update msg model =
    case msg of
        Increment ->
            Debug.log "increment" (model + 1)

        Decrement ->
            Debug.log "decrement" (model - 1)

        Reset ->
            Debug.log "reset" 0



-- VIEW


view : Model -> Html Msg
view model =
    div []
        [ div []
            [ button [ onClick Decrement ] [ text "-" ]
            , div [] [ text (String.fromInt model) ]
            , button [ onClick Increment ] [ text "+" ]
            ]
        , div []
            [ button [ onClick Reset ] [ text "RAZ" ] ]
        ]
