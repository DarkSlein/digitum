import { AxiosPromise } from 'axios';

import { NetworkList, NetworkSettings, NetworkStatus } from '../types';
import { AXIOS } from './endpoints';

export function readNetworkStatus(): AxiosPromise<NetworkStatus> {
  return AXIOS.get('/networkStatus');
}

export function scanNetworks(): AxiosPromise<void> {
  return AXIOS.get('/scanNetworks');
}

export function listNetworks(): AxiosPromise<NetworkList> {
  return AXIOS.get('/listNetworks');
}

export function readNetworkSettings(): AxiosPromise<NetworkSettings> {
  return AXIOS.get('/networkSettings');
}

export function updateNetworkSettings(NetworkSettings: NetworkSettings): AxiosPromise<NetworkSettings> {
  return AXIOS.post('/networkSettings', NetworkSettings);
}
