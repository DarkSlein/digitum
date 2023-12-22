import { AxiosPromise } from "axios";

import { IntercomJournal, IntercomSettings, IntercomStatus, SwitchDoorDTO } from "../types";
import { AXIOS } from "./endpoints";

export function readIntercomStatus(): AxiosPromise<IntercomStatus> {
  return AXIOS.get('/intercomStatus');
}

export function readIntercomJournal(): AxiosPromise<IntercomJournal> {
  return AXIOS.get('/intercomJournal');
}

export function readIntercomSettings(): AxiosPromise<IntercomSettings> {
  return AXIOS.get('/intercomSettings');
}

export function switchDoor(switchDoorDTO: SwitchDoorDTO): AxiosPromise<IntercomSettings> {
  return AXIOS.post('/switchDoor', switchDoorDTO);
}

export function updateIntercomSettings(intercomSettings: IntercomSettings): AxiosPromise<IntercomSettings> {
  return AXIOS.post('/intercomSettings', intercomSettings);
}
