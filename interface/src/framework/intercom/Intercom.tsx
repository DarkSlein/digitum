import React, { FC, useContext } from 'react';
import { Navigate, Routes, Route } from 'react-router-dom';

import { Tab } from '@mui/material';

import { RequireAdmin, RouterTabs, useLayoutTitle, useRouterTab } from '../../components';
import { AuthenticatedContext } from '../../contexts/authentication';

import IntercomStatusForm from './IntercomStatusForm';
import IntercomJournalForm from './IntercomJournalForm';
import IntercomSettingsForm from './IntercomSettingsForm';

const Intercom: FC = () => {
  useLayoutTitle("Access Point");

  const authenticatedContext = useContext(AuthenticatedContext);
  const { routerTab } = useRouterTab();

  return (
    <>
      <RouterTabs value={routerTab}>
        <Tab value="status" label="Intercom Status" />
        <Tab value="journal" label="Intercom Journal" />
        <Tab value="settings" label="Intercom Settings" disabled={!authenticatedContext.me.admin} />
      </RouterTabs>
      <Routes>
        <Route path="status" element={<IntercomStatusForm />} />
        <Route path="journal" element={<IntercomJournalForm />} />
        <Route
          path="settings"
          element={
            <RequireAdmin>
              <IntercomSettingsForm />
            </RequireAdmin>
          }
        />
        <Route path="/*" element={<Navigate replace to="status" />} />
      </Routes>
    </>
  );

};

export default Intercom;
