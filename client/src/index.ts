import * as monaco from 'monaco-editor';
import MonacoDiffEditor from "./diff";
import MonacoEditor from "./editor";

export * from "./types";

// eslint-disable-next-line no-restricted-exports
export { MonacoEditor as default, MonacoDiffEditor, monaco };
