import Schema from 'async-validator';

import { IntercomSettings } from '../types';

import { IP_ADDRESS_VALIDATOR } from './shared';

export const createIntercomSettingsValidator = (intercomSettings: IntercomSettings) => new Schema({
  kmnModel: [],
  kmnModelList: [],
  firstApartment: [
    { required: true, message: "Please provide a first appartment (for example, 1)" },
    { type: "number", message: "First appartment should be a number" }
  ]
});
