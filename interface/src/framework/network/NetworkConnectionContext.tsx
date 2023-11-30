import React from 'react';

import { Network } from '../../types';

export interface NetworkConnectionContextValue {
  selectedNetwork?: Network;
  selectNetwork: (network: Network) => void;
  deselectNetwork: () => void;
}

const NetworkConnectionContextDefaultValue = {} as NetworkConnectionContextValue;
export const NetworkConnectionContext = React.createContext(
  NetworkConnectionContextDefaultValue
);
