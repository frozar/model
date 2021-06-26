package com.example.demo.datasource;

import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.boot.jdbc.DataSourceBuilder;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import com.zaxxer.hikari.HikariDataSource;

// The @Configuration annotation tells spring to consider the class 
// PostgresDatasource as a configuration class, for Postgres DB in this case.

// The @Bean annotation tells spring to instanciate HikariDataSource as a bean.

// The @ConfigurationProperties tells spring to use the instance HikariDataSource
// as a configuration.

// This configuration is link to the setup in resources/application.yml

@Configuration
public class PostgresDatasource {

  @Bean
  @ConfigurationProperties("app.datasource")
  public HikariDataSource hikariDataSource () {
    return DataSourceBuilder.create().type(HikariDataSource.class).build();
  }
}
