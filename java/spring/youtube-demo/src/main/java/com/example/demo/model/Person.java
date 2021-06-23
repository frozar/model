package com.example.demo.model;


import java.util.UUID;

import com.fasterxml.jackson.annotation.JsonProperty;

// The @JsonProperty tells spring to match the constructor parameter
// to JSON payload field.

public class Person {

  private final UUID id;
  
  //@NotBlank
  // TODO: why @NotBlank is unavailable? (API update)
  private final String name;

  public Person(@JsonProperty("id") UUID id, 
      @JsonProperty("name") String name) {
    this.id = id;
    this.name = name;
  }

  public UUID getId() {
    return id;
  }

  public String getName() {
    return name;
  }

}
