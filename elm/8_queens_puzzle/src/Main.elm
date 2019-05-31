module Main exposing (Model, Msg(..), init, main, update, view)

import Browser
import Debug
import Html exposing (Html, button, div, span, text)
import Html.Attributes exposing (..)
import Html.Events exposing (onClick)


main =
    Browser.sandbox { init = init, update = update, view = view }



-- MODEL


type alias Queen =
    String


type alias Board =
    List (List (Maybe Queen))


type alias Model =
    { selectedAvailableQueen : Maybe Queen
    , selectedPlacedQueen : Maybe Queen
    , queens : List Queen
    , board : Board
    }


nbPiece =
    8


noneQueen : Queen
noneQueen =
    "qi"


initQueens : List Queen
initQueens =
    List.indexedMap
        (\i q -> "Q" ++ String.fromInt i)
        (List.repeat 8 noneQueen)


initBoard : Board
initBoard =
    List.repeat nbPiece (List.repeat nbPiece Nothing)


init : Model
init =
    { selectedAvailableQueen = Nothing
    , selectedPlacedQueen = Nothing
    , queens = initQueens
    , board = initBoard
    }



-- UPDATE


type Msg
    = Select Queen
    | SelectCell Int Int
    | Reset


update : Msg -> Model -> Model
update msg model =
    case msg of
        Select queen ->
            case model.selectedPlacedQueen of
                Just selectedPlacedQueen ->
                    { model
                        | queens = selectedPlacedQueen :: model.queens
                        , board = removeQueenBoard model.board selectedPlacedQueen
                        , selectedPlacedQueen = Nothing
                        , selectedAvailableQueen = Nothing
                    }

                Nothing ->
                    { model
                        | selectedAvailableQueen = Just queen
                        , selectedPlacedQueen = Nothing
                    }

        SelectCell i j ->
            case hasQueenAt model.board i j of
                Just queen ->
                    case model.selectedPlacedQueen of
                        Just alreadySelectedQueen ->
                            if alreadySelectedQueen == queen then
                                { model
                                    | board = removeQueenBoard model.board queen --placeQueen model.board queen i j
                                    , queens = alreadySelectedQueen :: model.queens
                                    , selectedPlacedQueen = Nothing
                                    , selectedAvailableQueen = Nothing
                                }

                            else
                                { model
                                    | selectedPlacedQueen = Just queen
                                    , selectedAvailableQueen = Nothing
                                }

                        Nothing ->
                            { model
                                | selectedPlacedQueen = Just queen
                                , selectedAvailableQueen = Nothing
                            }

                Nothing ->
                    case model.selectedAvailableQueen of
                        Just queen ->
                            { model
                                | queens = removeQueenAvailable queen model.queens
                                , board = placeQueen model.board queen i j
                                , selectedAvailableQueen = Nothing
                                , selectedPlacedQueen = Nothing
                            }

                        Nothing ->
                            case model.selectedPlacedQueen of
                                Just queen ->
                                    { model
                                        | board = placeQueen model.board queen i j
                                        , selectedAvailableQueen = Nothing
                                        , selectedPlacedQueen = Nothing
                                    }

                                Nothing ->
                                    model

        Reset ->
            Debug.log "reset" { model | selectedAvailableQueen = Nothing, selectedPlacedQueen = Nothing, queens = initQueens, board = initBoard }


hasQueenAt : Board -> Int -> Int -> Maybe Queen
hasQueenAt board iPlace jPlace =
    case List.head (Debug.log "hasQueenAt i" (List.reverse (List.take (iPlace + 1) board))) of
        Just listQueen ->
            case List.head (Debug.log "hasQueenAt j" (List.reverse (List.take (jPlace + 1) listQueen))) of
                Just maybeQueen ->
                    Debug.log "hasQueenAt - maybeQueen"
                        maybeQueen

                Nothing ->
                    Debug.log "hasQueenAt - Nothing 1"
                        Nothing

        Nothing ->
            Debug.log "hasQueenAt - Nothing 0"
                Nothing


removeQueenBoard : Board -> Queen -> Board
removeQueenBoard board queen =
    List.map
        (\listQueen ->
            List.map
                (\maybeQueen ->
                    case maybeQueen of
                        Just q ->
                            if q == queen then
                                Nothing

                            else
                                Just q

                        Nothing ->
                            Nothing
                )
                listQueen
        )
        board


placeQueen : Board -> Queen -> Int -> Int -> Board
placeQueen board queen iPlace jPlace =
    List.indexedMap
        (\i listQueen ->
            List.indexedMap
                (\j maybeQueen ->
                    if (i == iPlace) && (j == jPlace) then
                        -- Just { queen | selected = False }
                        Just queen

                    else
                        case maybeQueen of
                            Just q ->
                                if q == queen then
                                    Nothing

                                else
                                    Just q

                            Nothing ->
                                Nothing
                )
                listQueen
        )
        board


removeQueenAvailable : Queen -> List Queen -> List Queen
removeQueenAvailable queen listQueen =
    List.filter (\q -> q /= queen) listQueen


isSelected : Model -> Queen -> Bool
isSelected model queen =
    case model.selectedAvailableQueen of
        Just q1 ->
            if q1 == queen then
                True

            else
                case model.selectedPlacedQueen of
                    Just q2 ->
                        if q2 == queen then
                            True

                        else
                            False

                    Nothing ->
                        False

        Nothing ->
            case model.selectedPlacedQueen of
                Just q ->
                    if q == queen then
                        True

                    else
                        False

                Nothing ->
                    False



-- VIEW


view : Model -> Html Msg
view model =
    div []
        [ div []
            [ button [ onClick Reset ] [ text "RAZ" ] ]
        , div []
            [ div [] (List.map (viewQueen True model) model.queens)
            ]
        , viewBoard model
        ]


viewBoard : Model -> Html Msg
viewBoard model =
    Html.tbody
        [ style "celingspace" "0px"
        , style "border-spacing" "0px"
        , style "border-collapse" "collapse"
        , style "margin-left" "auto"
        , style "margin-right" "auto"
        ]
        (List.indexedMap
            (\i listQueen ->
                Html.tr []
                    (List.indexedMap
                        (\j queen ->
                            Html.td
                                [ style "border" "1px solid #505050"
                                , style "text-align" "center"
                                , style "padding" "0px"
                                , style "width" "2rem"
                                , style "height" "2rem"
                                , style "background-color"
                                    (if modBy 2 (i + j) == 0 then
                                        "black"

                                     else
                                        "white"
                                    )
                                , onClick (SelectCell i j)
                                ]
                                [ case queen of
                                    Just q ->
                                        span
                                            [ style "color"
                                                (if isSelected model q then
                                                    "red"

                                                 else if modBy 2 (i + j) == 0 then
                                                    "white"

                                                 else
                                                    "black"
                                                )
                                            ]
                                            [ text q ]

                                    Nothing ->
                                        span
                                            [ style "color"
                                                (if modBy 2 (i + j) == 0 then
                                                    "white"

                                                 else
                                                    "black"
                                                )
                                            ]
                                            [ text "X" ]
                                ]
                        )
                        listQueen
                    )
            )
            model.board
        )


viewQueen : Bool -> Model -> Queen -> Html Msg
viewQueen debug model queen =
    div
        [ onClick (Select queen)
        , style "color"
            (if isSelected model queen then
                "red"

             else
                "black"
            )
        ]
        [ text queen ]
