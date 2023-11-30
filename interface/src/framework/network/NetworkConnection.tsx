import React, { FC, useCallback, useContext, useState } from 'react';
import { Navigate, Routes, Route, useNavigate } from 'react-router-dom';

import { Tab } from '@mui/material';

import { RequireAdmin, RouterTabs, useLayoutTitle, useRouterTab } from '../../components';
import { Network } from '../../types';
import { AuthenticatedContext } from '../../contexts/authentication';
import { NetworkConnectionContext } from './NetworkConnectionContext';
import NetworkStatusForm from './NetworkStatusForm';
import NetworkScanner from './NetworkScanner';
import NetworkSettingsForm from './NetworkSettingsForm';

const NetworkConnection: FC = () => {
  useLayoutTitle("Network");

  const authenticatedContext = useContext(AuthenticatedContext);
  const navigate = useNavigate();
  const { routerTab } = useRouterTab();

  const [selectedNetwork, setSelectedNetwork] = useState<Network>();

  const selectNetwork = useCallback((network: Network) => {
    setSelectedNetwork(network);
    navigate('settings');
  }, [navigate]);

  const deselectNetwork = useCallback(() => {
    setSelectedNetwork(undefined);
  }, []);

  return (
    <NetworkConnectionContext.Provider
      value={{
        selectedNetwork,
        selectNetwork,
        deselectNetwork
      }}
    >
      <RouterTabs value={routerTab}>
        <Tab value="status" label="Network Status" />
        <Tab value="settings" label="Network Settings" disabled={!authenticatedContext.me.admin} />
      </RouterTabs>
      <Routes>
        <Route path="status" element={<NetworkStatusForm />} />
        <Route
          path="settings"
          element={
            <RequireAdmin>
              <NetworkSettingsForm />
            </RequireAdmin>
          }
        />
        <Route path="/*" element={<Navigate replace to="status" />} />
      </Routes>
    </NetworkConnectionContext.Provider>
  );

};

export default NetworkConnection;
