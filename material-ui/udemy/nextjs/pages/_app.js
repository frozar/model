import React, { useState } from "react";
import ReactGA from "react-ga";
import PropTypes from "prop-types";
import Head from "next/head";
import { ThemeProvider } from "@material-ui/core/styles";
import Theme from "../src/ui/Theme";

import Header from "../src/ui/Header";
import Footer from "../src/ui/Footer";

// TODO: update this field
ReactGA.initialize("GOOGLE-ANALYTICS-ID");

export default function MyApp(props) {
  const { Component, pageProps } = props;

  const [selectedIndex, setSelectedIndex] = useState(0);
  const [value, setValue] = useState(0);

  React.useEffect(() => {
    // Remove the server-side injected CSS.
    const jssStyles = document.querySelector("#jss-server-side");
    if (jssStyles) {
      jssStyles.parentElement.removeChild(jssStyles);
    }
  }, []);

  return (
    <React.Fragment>
      <Head>
        <title>Arc development</title>
        <meta
          name="viewport"
          content="minimum-scale=1, initial-scale=1, width=device-width"
        />
      </Head>
      <ThemeProvider theme={Theme}>
        {/* CssBaseline kickstart an elegant, consistent, and simple baseline to build upon. */}
        <Header
          value={value}
          setValue={setValue}
          selectedIndex={selectedIndex}
          setSelectedIndex={setSelectedIndex}
        />
        <Component
          {...pageProps}
          setValue={setValue}
          setSelectedIndex={setSelectedIndex}
        />
        <Footer setValue={setValue} setSelectedIndex={setSelectedIndex} />
      </ThemeProvider>
    </React.Fragment>
  );
}

MyApp.propTypes = {
  Component: PropTypes.elementType.isRequired,
  pageProps: PropTypes.object.isRequired,
};
