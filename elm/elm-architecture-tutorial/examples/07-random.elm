module Main exposing (Model, Msg(..), init, main, subscriptions, update, view, viewDice, viewDiceDot)

import Browser
import Html exposing (..)
import Html.Attributes exposing (..)
import Html.Events exposing (..)
import Process
import Random
import Svg exposing (..)
import Svg.Attributes exposing (..)
import Task



-- MAIN


main =
    Browser.element
        { init = init
        , update = update
        , subscriptions = subscriptions
        , view = view
        }



-- MODEL


type alias Model =
    { dieFace1 : Int
    , dieFace2 : Int
    , rolling : Bool
    }


init : () -> ( Model, Cmd Msg )
init _ =
    ( Model 1 6 False
    , Cmd.none
    )



-- UPDATE


type Msg
    = Roll
    | Rolled Int Int Int
    | Continue Int


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        Roll ->
            ( { model | rolling = True }
            , roll 3
            )

        Rolled 0 value1 value2 ->
            ( { model | rolling = False, dieFace1 = value1, dieFace2 = value2 }
            , Cmd.none
            )

        Rolled nbBounces value1 value2 ->
            ( { model | dieFace1 = value1, dieFace2 = value2 }
            , Process.sleep 1000 |> Task.perform (\_ -> Continue nbBounces)
            )

        Continue nbBounces ->
            ( model
            , roll nbBounces
            )


oneToSix : Random.Generator Int
oneToSix =
    Random.weighted ( 15, 1 ) [ ( 15, 2 ), ( 15, 3 ), ( 15, 4 ), ( 15, 5 ), ( 100 - (5 * 15), 6 ) ]


roll : Int -> Cmd Msg
roll nbBounces =
    Random.map3 Rolled (Random.constant (nbBounces - 1)) oneToSix oneToSix
        |> Random.generate identity



-- SUBSCRIPTIONS


subscriptions : Model -> Sub Msg
subscriptions model =
    Sub.none



-- VIEW


view : Model -> Html Msg
view model =
    div [ Html.Attributes.style "text-align" "center" ]
        [ div []
            [ viewDice model.dieFace1
            , viewDice model.dieFace2
            ]
        , div [] [ Html.text (String.fromInt model.dieFace1) ]
        , div [] [ Html.text (String.fromInt model.dieFace2) ]
        , button [ onClick Roll, disabled model.rolling ] [ Html.text "Roll" ]
        ]


viewDice : Int -> Html Msg
viewDice dieFace =
    let
        length_i =
            150

        offset =
            2

        dice_min_i =
            0 + offset

        dice_length_i =
            length_i - (2 * offset)

        length_s =
            String.fromInt length_i

        dice_min_s =
            String.fromInt dice_min_i

        dice_length_s =
            String.fromInt dice_length_i
    in
    svg
        [ Svg.Attributes.width length_s
        , Svg.Attributes.height length_s
        , viewBox ("0 0 " ++ length_s ++ " " ++ length_s)
        , fill "black"
        , stroke "black"
        , strokeWidth "3"
        ]
        (List.append
            [ rect
                [ x dice_min_s
                , y dice_min_s
                , fill "white"
                , Svg.Attributes.width dice_length_s
                , Svg.Attributes.height dice_length_s
                , rx "10"
                , ry "10"
                ]
                []
            ]
            (viewDiceDot dice_min_i dice_length_i dieFace)
        )


viewDiceDot : Int -> Int -> Int -> List (Html Msg)
viewDiceDot dice_min_i dice_length_i value =
    let
        dice_center_i =
            (dice_length_i - dice_min_i) // 2 + dice_min_i

        dice_center_s =
            String.fromInt dice_center_i

        diagNW_x =
            String.fromInt (dice_center_i - (dice_length_i // 3))

        diagNW_y =
            diagNW_x

        diagSE_x =
            String.fromInt (dice_center_i + (dice_length_i // 3))

        diagSE_y =
            diagSE_x

        diagNE_x =
            diagSE_x

        diagNE_y =
            diagNW_y

        diagSW_x =
            diagNW_x

        diagSW_y =
            diagSE_y
    in
    case value of
        1 ->
            [ circleSvg dice_center_s dice_center_s
            ]

        2 ->
            [ circleSvg diagNW_x diagNW_y
            , circleSvg diagSE_x diagSE_y
            ]

        3 ->
            [ circleSvg diagNW_x diagNW_y
            , circleSvg dice_center_s dice_center_s
            , circleSvg diagSE_x diagSE_y
            ]

        4 ->
            [ circleSvg diagNW_x diagNW_y
            , circleSvg diagNE_x diagNE_y
            , circleSvg diagSW_x diagSW_y
            , circleSvg diagSE_x diagSE_y
            ]

        5 ->
            [ circleSvg dice_center_s dice_center_s
            , circleSvg diagNW_x diagNW_y
            , circleSvg diagNE_x diagNE_y
            , circleSvg diagSW_x diagSW_y
            , circleSvg diagSE_x diagSE_y
            ]

        6 ->
            [ circleSvg diagNW_x diagNW_y
            , circleSvg diagNE_x diagNE_y
            , circleSvg diagNW_x dice_center_s
            , circleSvg diagNE_x dice_center_s
            , circleSvg diagSW_x diagSW_y
            , circleSvg diagSE_x diagSE_y
            ]

        _ ->
            [ circle
                [ cx dice_center_s
                , cy dice_center_s
                , r "60"
                , fill "red"
                ]
                []
            ]


circleSvg : String -> String -> Html Msg
circleSvg cx_ cy_ =
    circle [ cx cx_, cy cy_, r "10" ] []
