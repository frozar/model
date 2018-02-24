#include <jsoncpp/json/json.h>
#include <iostream>

int main( ) {
    Json::Value event ;
    Json::Value vec(Json::arrayValue) ;
    vec.append(Json::Value(1)) ;
    vec.append(Json::Value(2)) ;
    vec.append(Json::Value(3)) ;

    event["competitors"]["home"]["name"] = "Liverpool" ;
    event["competitors"]["away"]["code"] = 89223 ;
    event["competitors"]["away"]["name"] = "Aston Villa" ;
    event["competitors"]["away"]["code"] = vec ;

    std::cout << event << std::endl ;
}

// g++ json.cpp -o json -ljsoncpp && ./json
