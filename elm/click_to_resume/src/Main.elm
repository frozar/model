module Main exposing (..)

import Browser
import Html exposing (button, div, text)
import Html.Attributes exposing (id)
import Html.Events exposing (onClick)

main = Browser.sandbox { init = 0 , view = view , update = update }

type Msg = Increment

update _ n = n + 1

view n =
    div []
        [ text (String.fromInt n)
        , button [ onClick Increment ] [ text "+" ]
        ]
