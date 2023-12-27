import React from "react";
import ReactDOM from "react-dom/client";
import { MonacoDiffEditor } from "./index";

const value = ["function x() {", '\tconsole.log("Hello world!");', "}"].join("\n");
const original = ["function y() {", '\tconsole.log("Hello world!");', "}"].join("\n");

const root = ReactDOM.createRoot(document.getElementById("root")!);
root.render(
  <React.StrictMode>
    <MonacoDiffEditor value={value} original={original} width="auto" height="300" />
  </React.StrictMode>
);

// If you want to start measuring performance in your app, pass a function
// to log results (for example: reportWebVitals(console.log))
// or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals
// const reportWebVitals = (onPerfEntry?: any) => {
//   if (onPerfEntry && onPerfEntry instanceof Function) {
//     import("web-vitals").then(({ getCLS, getFID, getFCP, getLCP, getTTFB }) => {
//       getCLS(onPerfEntry);
//       getFID(onPerfEntry);
//       getFCP(onPerfEntry);
//       getLCP(onPerfEntry);
//       getTTFB(onPerfEntry);
//     });
//   }
// };
// reportWebVitals();
