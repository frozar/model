module Main exposing (Model, Msg(..), init, main, update, view)

import Browser
import Debug
import Set
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


type SelectedQueen
    = Available Queen
    | Placed Queen
    | NothingSelected


type alias Model =
    { selectedQueen : SelectedQueen
    , queens : List Queen
    , board : Board
    , queenAgainst : (Set.Set ( ( Queen, Queen ) ) )
    }


nbPiece =
    8


noneQueen : Queen
noneQueen =
    "xx"


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
    { selectedQueen = NothingSelected
    , queens = initQueens
    , board = initBoard
    , queenAgainst = (Set.empty)
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
            case model.selectedQueen of
                Placed selectedPlacedQueen ->
                    { model
                        | queens = selectedPlacedQueen :: model.queens
                        , board = removeQueenBoard model.board selectedPlacedQueen
                        , selectedQueen = NothingSelected
                    }

                Available selectedAvailableQueen ->
                    { model
                        | selectedQueen = Available queen
                    }

                NothingSelected ->
                    { model
                        | selectedQueen = Available queen
                    }

        SelectCell i j ->
            case hasQueenAt model.board i j of
                Just queen ->
                    case model.selectedQueen of
                        Placed alreadySelectedQueen ->
                            if alreadySelectedQueen == queen then
                                { model
                                    | board = removeQueenBoard model.board queen --placeQueen model.board queen i j
                                    , queens = alreadySelectedQueen :: model.queens
                                    , selectedQueen = NothingSelected
                                }

                            else
                                { model
                                    | selectedQueen = Placed queen
                                }

                        Available alreadySelectedQueen ->
                            { model
                                | selectedQueen = Placed queen
                            }

                        NothingSelected ->
                            { model
                                | selectedQueen = Placed queen
                            }

                Nothing ->
                    case model.selectedQueen of
                        Available queen ->
                            { model
                                | queens = removeQueenAvailable queen model.queens
                                , board = placeQueen model.board queen i j
                                , selectedQueen = NothingSelected
                            }

                        Placed queen ->
                            { model
                                | board = placeQueen model.board queen i j
                                , selectedQueen = NothingSelected
                            }

                        NothingSelected ->
                            model

        Reset ->
            { model
                | selectedQueen = NothingSelected
                , queens = initQueens
                , board = initBoard
            }


hasQueenAt : Board -> Int -> Int -> Maybe Queen
hasQueenAt board iPlace jPlace =
    case List.head (List.reverse (List.take (iPlace + 1) board)) of
        Just listQueen ->
            case List.head (List.reverse (List.take (jPlace + 1) listQueen)) of
                Just maybeQueen ->
                    maybeQueen

                Nothing ->
                    Nothing

        Nothing ->
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


isSelected : SelectedQueen -> Queen -> Bool
isSelected selectedQueen queen =
    case selectedQueen of
        Available q1 ->
            q1 == queen

        Placed q1 ->
            q1 == queen

        NothingSelected ->
            False



-- VIEW


view : Model -> Html Msg
view model =
    div []
        [ div []
            [ button [ onClick Reset ] [ text "RAZ" ] ]
        , viewBoard model.board model.selectedQueen
        , div []
            [ div [] (List.map (viewQueen model.selectedQueen) model.queens)
            ]
        ]


viewBoard : Board -> SelectedQueen -> Html Msg
viewBoard board selectedQueen =
    Html.tbody
        []
        (List.indexedMap
            (\i listQueen ->
                Html.tr []
                    (List.indexedMap
                        (\j maybeQueen ->
                            viewBoardCell selectedQueen maybeQueen i j
                        )
                        listQueen
                    )
            )
            board
        )


viewBoardCell : SelectedQueen -> Maybe Queen -> Int -> Int -> Html Msg
viewBoardCell selectedQueen maybeQueen i j =
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
        [ case maybeQueen of
            Just queen ->
                span
                    [ style "color"
                        (if isSelected selectedQueen queen then
                            "red"

                         else if modBy 2 (i + j) == 0 then
                            "white"

                         else
                            "black"
                        )
                    ]
                    [ text queen ]

            Nothing ->
                span
                    [ style "color"
                        (if modBy 2 (i + j) == 0 then
                            "white"

                         else
                            "black"
                        )
                    ]
                    [ text "" ]
        ]


viewQueen : SelectedQueen -> Queen -> Html Msg
viewQueen selectedQueen queen =
    div
        [ onClick (Select queen)
        , style "color"
            (if isSelected selectedQueen queen then
                "red"

             else
                "black"
            )
        ]
        [ text queen ]
