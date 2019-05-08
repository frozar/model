import Browser
import Html exposing (Html, button, div, text)
import Html.Events exposing (onClick)


main =
  Browser.sandbox { init = init, update = update, view = view }


-- MODEL

type alias Model = Int

init : Model
init =
  2 ^ 1023 + 2 ^ 1022


-- UPDATE

type Msg = Increment | Decrement | Reset

update : Msg -> Model -> Model
update msg model =
  case msg of
    Increment ->
      model + 1

    Decrement ->
      model - 1

    Reset ->
      0


-- VIEW

view : Model -> Html Msg
view model =
  div []
  [ 
    div [] 
    [ button [ onClick Decrement ] [ text "-" ]
    , div [] [ text (String.fromInt model) ]
    , button [ onClick Increment ] [ text "+" ]
    ],
    div [] 
    [ button [ onClick Reset ] [ text "RAZ" ] ]
  ]
