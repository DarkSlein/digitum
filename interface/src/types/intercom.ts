export enum IntercomConnectionStatus {
  CONNECTED = 0,
  NOT_CONNECTED = 1,
  RECEIVING_DATA = 2
}

export enum DoorStatus {
  OPENED = 0,
  CLOSED = 1,
}

export enum SwitchDoorType {
  ON_OFF = 0,
  JOGGING = 1,
  DELAY = 2,
}

export interface IntercomStatus {
  status: IntercomConnectionStatus;
  lastCalledNumber: number;
  doorStatus: DoorStatus;
}

export interface IntercomJournalNote {
  createdAt: string;
  appartment: number;
}

export interface IntercomJournal {
  notes: IntercomJournalNote[];
}

export interface IntercomSettings {
  kmnModel: string;
  kmnModelList: string[];
  firstAppartment: number;
}

export interface SwitchDoorDTO {
  type: SwitchDoorType;
  status: DoorStatus;
  time: number;
}