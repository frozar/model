module Main exposing (Model, Msg(..), init, main, update, view, viewConverter)

import Browser
import Html exposing (Attribute, Html, div, input, span, text)
import Html.Attributes exposing (..)
import Html.Events exposing (onInput)



-- MAIN


main =
    Browser.sandbox { init = init, update = update, view = view }



-- MODEL


type alias Model =
    { inputCelsius : String
    , inputFahrenheit : String
    , inputInch : String
    }


init : Model
init =
    { inputCelsius = ""
    , inputFahrenheit = "" 
    , inputInch = "" 
    }



-- UPDATE


type Msg
    = ChangeCelsius String
    | ChangeFahrenheit String
    | ChangeInch String


update : Msg -> Model -> Model
update msg model =
    case msg of
        ChangeCelsius newInput ->
            { model | inputCelsius = newInput }

        ChangeFahrenheit newInput ->
            { model | inputFahrenheit = newInput }

        ChangeInch newInput ->
            { model | inputInch = newInput }



-- VIEW


view : Model -> Html Msg
view model =
    div []
        [ div []
            [ viewCelsius model ]
        , div []
            [ viewFahrenheit model ]
        , div []
            [ viewInch model ]
        ]


viewCelsius : Model -> Html Msg
viewCelsius model =
    viewConverterCelsius model (String.toFloat model.inputCelsius)


viewFahrenheit : Model -> Html Msg
viewFahrenheit model =
    viewConverterFahrenheit model (String.toFloat model.inputFahrenheit)


viewInch : Model -> Html Msg
viewInch model =
    viewConverterInch model (String.toFloat model.inputInch)


viewConverterInch : Model -> Maybe Float -> Html Msg
viewConverterInch model maybeInch =
    let
        converterHelper : String -> String -> Html Msg
        converterHelper =
            viewConverter "in" "m" model.inputInch ChangeInch

        inchToMeter : Float -> String
        inchToMeter inch =
            String.fromFloat (inch * 0.0254)
    in
    case maybeInch of
        Just inch ->
            converterHelper "blue" (inchToMeter inch)

        Nothing ->
            converterHelper "red" "???"


viewConverterCelsius : Model -> Maybe Float -> Html Msg
viewConverterCelsius model maybeCelsius =
    let
        converterHelper : String -> String -> Html Msg
        converterHelper =
            viewConverter "°C" "°F" model.inputCelsius ChangeCelsius

        celsiusToFahrenheit : Float -> String
        celsiusToFahrenheit celsius =
            String.fromFloat (celsius * 1.8 + 32)
    in
    case maybeCelsius of
        Just celsius ->
            converterHelper "blue" (celsiusToFahrenheit celsius)

        Nothing ->
            converterHelper "red" "???"


viewConverterFahrenheit : Model -> Maybe Float -> Html Msg
viewConverterFahrenheit model maybeFahrenheit =
    let
        converterHelper : String -> String -> Html Msg
        converterHelper =
            viewConverter "°F" "°C" model.inputFahrenheit ChangeFahrenheit

        fahrenheitToCelsius : Float -> String
        fahrenheitToCelsius fahrenheit =
            String.fromFloat ((fahrenheit - 32) / 1.8)
    in
    case maybeFahrenheit of
        Just fahrenheit ->
            converterHelper "blue" (fahrenheitToCelsius fahrenheit)

        Nothing ->
            converterHelper "red" "???"


viewConverter : String -> String -> String -> (String -> Msg) -> String -> String -> Html Msg
viewConverter inUnit outUnit userInput msg color equivalentTemp =
    span []
        [ input [ value userInput, onInput msg, style "width" "40px", style "border-color" color ] []
        , text (inUnit ++ " = ")
        , span [ style "color" color ] [ text equivalentTemp ]
        , text outUnit
        ]
