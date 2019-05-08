module Main exposing (Model, Msg(..), init, main, update, view, viewInput, viewValidation)

import Browser
import Char exposing (isDigit, isLower, isUpper)
import Html exposing (..)
import Html.Attributes exposing (..)
import Html.Events exposing (onClick, onInput, onSubmit)
import String exposing (any)



-- MAIN


main =
    Browser.sandbox { init = init, update = update, view = view }



-- MODEL


type alias Model =
    { name : String
    , password : String
    , passwordAgain : String
    , age : String
    , displayValidity : Bool
    }


init : Model
init =
    Model "" "" "" "" False



-- UPDATE


type Msg
    = Name String
    | Password String
    | PasswordAgain String
    | Age String
    | CheckForm


update : Msg -> Model -> Model
update msg model =
    case msg of
        Name name ->
            { model | name = name, displayValidity = False }

        Password password ->
            { model | password = password, displayValidity = False }

        PasswordAgain password ->
            { model | passwordAgain = password, displayValidity = False }

        Age age ->
            { model | age = age, displayValidity = False }

        CheckForm ->
            { model | displayValidity = True }



-- VIEW


view : Model -> Html Msg
view model =
    div []
        [ viewInput "text" "Name" model.name Name
        , viewInput "password" "Password" model.password Password
        , viewInput "password" "Re-enter Password" model.passwordAgain PasswordAgain
        , viewInput "age" "Age" model.age Age
        , button [ onClick CheckForm ] [ text "Fake submit form" ]
        , viewValidation model
        ]


viewInput : String -> String -> String -> (String -> msg) -> Html msg
viewInput t p v toMsg =
    input [ type_ t, placeholder p, value v, onInput toMsg ] []


viewValidation : Model -> Html msg
viewValidation model =
    if model.displayValidity then
        if
            passwordCheckRepeat model.password model.passwordAgain
                && passwordCheckLength model.password
                && passwordCheckConstraint model.password
                && ageIsInt model.age
        then
            div [ style "color" "green" ] [ text "Submission accepted" ]

        else if not (passwordCheckRepeat model.password model.passwordAgain) then
            div [ style "color" "red" ] [ text "Submitted passwords don't match" ]

        else if not (passwordCheckLength model.password) then
            div [ style "color" "red" ] [ text "Submitted password not long enough (more than 8 chars requiered)" ]

        else if not (passwordCheckConstraint model.password) then
            div [ style "color" "red" ] [ text "Submitted password doesn't have at least 1 lower case, 1 upper case and 1 digit" ]

        else if not (ageIsInt model.age) then
            div [ style "color" "red" ] [ text "Submitted age has to be a integer" ]

        else
            div [ style "color" "red" ] [ text "Invalid form" ]

    else
        div [ style "color" "black" ] [ text "Wait for submission" ]


passwordCheckRepeat : String -> String -> Bool
passwordCheckRepeat password passwordAgain =
    password == passwordAgain


passwordCheckLength : String -> Bool
passwordCheckLength password =
    String.length password >= 8


passwordCheckConstraint : String -> Bool
passwordCheckConstraint password =
    any isDigit password
        && any isUpper password
        && any isLower password


ageIsInt : String -> Bool
ageIsInt age =
    case String.toInt age of
        Just _ ->
            True

        Nothing ->
            False
