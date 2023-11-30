import { AxiosPromise } from "axios";

import { APSettings, APStatus } from "../types";
import { AXIOS } from "./endpoints";

export function readAPStatus(): AxiosPromise<APStatus> {
  return AXIOS.get('/intercomStatus');
}

export function readAPSettings(): AxiosPromise<APSettings> {
  return AXIOS.get('/intercomSettings');
}

export function updateAPSettings(apSettings: APSettings): AxiosPromise<APSettings> {
  return AXIOS.post('/intercomSettings', apSettings);
}
