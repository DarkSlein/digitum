import Schema from "async-validator";

import { NetworkSettings } from "../types";

import { HOSTNAME_VALIDATOR, IP_ADDRESS_VALIDATOR } from "./shared";

export const createNetworkSettingsValidator = (NetworkSettings: NetworkSettings) => new Schema({
  ...(NetworkSettings.static_ip_config && {
    local_ip: [
      { required: true, message: "Local IP is required" },
      IP_ADDRESS_VALIDATOR
    ],
    gateway_ip: [
      { required: true, message: "Gateway IP is required" },
      IP_ADDRESS_VALIDATOR
    ],
    subnet_mask: [
      { required: true, message: "Subnet mask is required" },
      IP_ADDRESS_VALIDATOR
    ],
    dns_ip_1: IP_ADDRESS_VALIDATOR,
    dns_ip_2: IP_ADDRESS_VALIDATOR,
  })
});
