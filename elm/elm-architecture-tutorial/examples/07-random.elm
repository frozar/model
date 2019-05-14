module Main exposing (Model, Msg(..), init, main, roll, subscriptions, update, view, viewDice, viewDiceDot)

import Browser
import Html exposing (..)
import Html.Attributes exposing (..)
import Html.Events exposing (..)
import Process
import Task
import Random
import Svg exposing (..)
import Svg.Attributes exposing (..)



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
    }


init : () -> ( Model, Cmd Msg )
init _ =
    ( Model 1 6
    , Cmd.none
    )



-- UPDATE


type Msg
    = Roll
    | NewFace10 Int
    | NewFace11 Int
    | NewFace2 Int


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        Roll ->
            ( model
            , Random.generate NewFace10 roll
            )

        NewFace10 newFace ->
            ( { model | dieFace1 = newFace }
            , Random.generate NewFace11 roll
            -- , Process.sleep 100 |> Task.perform ((Random.generate NewFace11 roll) ())
            -- , Process.sleep 1000 |> ((Task.perform (\_ -> Random.generate NewFace11 roll)) ())
            )

        NewFace11 newFace ->
            ( { model | dieFace1 = newFace }
            , Random.generate NewFace2 roll
            )

        NewFace2 newFace ->
            ( { model | dieFace2 = newFace }
            , Cmd.none
            )


roll : Random.Generator Int
roll =
    Random.weighted ( 15, 1 ) [ ( 15, 2 ), ( 15, 3 ), ( 15, 4 ), ( 15, 5 ), ( 100 - (5 * 15), 6 ) ]



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
        , button [ onClick Roll ] [ Html.text "Roll" ]
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
            [ circle
                [ cx dice_center_s
                , cy dice_center_s
                , r "10"
                ]
                []
            ]

        2 ->
            [ circle
                [ cx diagNW_x
                , cy diagNW_y
                , r "10"
                ]
                []
            , circle
                [ cx diagSE_x
                , cy diagSE_y
                , r "10"
                ]
                []
            ]

        3 ->
            [ circle
                [ cx diagNW_x
                , cy diagNW_y
                , r "10"
                ]
                []
            , circle
                [ cx dice_center_s
                , cy dice_center_s
                , r "10"
                ]
                []
            , circle
                [ cx diagSE_x
                , cy diagSE_y
                , r "10"
                ]
                []
            ]

        4 ->
            [ circle
                [ cx diagNW_x
                , cy diagNW_y
                , r "10"
                ]
                []
            , circle
                [ cx diagSW_x
                , cy diagSW_y
                , r "10"
                ]
                []
            , circle
                [ cx diagNE_x
                , cy diagNE_y
                , r "10"
                ]
                []
            , circle
                [ cx diagSE_x
                , cy diagSE_y
                , r "10"
                ]
                []
            ]

        5 ->
            [ circle
                [ cx dice_center_s
                , cy dice_center_s
                , r "10"
                ]
                []
            , circle
                [ cx diagNW_x
                , cy diagNW_y
                , r "10"
                ]
                []
            , circle
                [ cx diagSW_x
                , cy diagSW_y
                , r "10"
                ]
                []
            , circle
                [ cx diagNE_x
                , cy diagNE_y
                , r "10"
                ]
                []
            , circle
                [ cx diagSE_x
                , cy diagSE_y
                , r "10"
                ]
                []
            ]

        6 ->
            [ circle
                [ cx diagNW_x
                , cy diagNW_y
                , r "10"
                ]
                []
            , circle
                [ cx diagNE_x
                , cy diagNE_y
                , r "10"
                ]
                []
            , circle
                [ cx diagNW_x
                , cy dice_center_s
                , r "10"
                ]
                []
            , circle
                [ cx diagNE_x
                , cy dice_center_s
                , r "10"
                ]
                []
            , circle
                [ cx diagSW_x
                , cy diagSW_y
                , r "10"
                ]
                []
            , circle
                [ cx diagSE_x
                , cy diagSE_y
                , r "10"
                ]
                []
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
