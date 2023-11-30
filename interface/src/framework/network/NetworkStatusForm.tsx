import { FC } from "react";

import { Avatar, Button, Divider, List, ListItem, ListItemAvatar, ListItemText, Theme, useTheme } from "@mui/material";
import CableIcon from '@mui/icons-material/Cable';
import SpeedIcon from '@mui/icons-material/Speed';
import LanIcon from '@mui/icons-material/Lan';
import SettingsInputComponentIcon from '@mui/icons-material/SettingsInputComponent';
import DeviceHubIcon from '@mui/icons-material/DeviceHub';
import DnsIcon from '@mui/icons-material/Dns';
import RefreshIcon from '@mui/icons-material/Refresh';

import * as NetworkApi from "../../api/network";
import { NetworkConnectionStatus, NetworkStatus } from "../../types";
import { ButtonRow, FormLoader, SectionContent } from "../../components";
import { useRest } from "../../utils";

const isConnected = ({ status }: NetworkStatus) => status === NetworkConnectionStatus.NETWORK_STATUS_CONNECTED;

const NetworkStatusHighlight = ({ status }: NetworkStatus, theme: Theme) => {
  switch (status) {
    case NetworkConnectionStatus.NETWORK_STATUS_IDLE:
    case NetworkConnectionStatus.NETWORK_STATUS_DISCONNECTED:
    case NetworkConnectionStatus.NETWORK_STATUS_NO_SHIELD:
      return theme.palette.info.main;
    case NetworkConnectionStatus.NETWORK_STATUS_CONNECTED:
      return theme.palette.success.main;
    case NetworkConnectionStatus.NETWORK_STATUS_CONNECT_FAILED:
    case NetworkConnectionStatus.NETWORK_STATUS_CONNECTION_LOST:
      return theme.palette.error.main;
    default:
      return theme.palette.warning.main;
  }
};

const TransferNetworkStatus = ({ status }: NetworkStatus) => {
  switch (status) {
    case NetworkConnectionStatus.NETWORK_STATUS_NO_SHIELD:
      return "Inactive";
    case NetworkConnectionStatus.NETWORK_STATUS_IDLE:
      return "Idle";
    case NetworkConnectionStatus.NETWORK_STATUS_NO_SSID_AVAIL:
      return "No SSID Available";
    case NetworkConnectionStatus.NETWORK_STATUS_CONNECTED:
      return "Connected";
    case NetworkConnectionStatus.NETWORK_STATUS_CONNECT_FAILED:
      return "Connection Failed";
    case NetworkConnectionStatus.NETWORK_STATUS_CONNECTION_LOST:
      return "Connection Lost";
    case NetworkConnectionStatus.NETWORK_STATUS_DISCONNECTED:
      return "Disconnected";
    default:
      return "Unknown";
  }
};

const NetworkStatusForm: FC = () => {
  const {
    loadData, data, errorMessage
  } = useRest<NetworkStatus>({ read: NetworkApi.readNetworkStatus });

  const theme = useTheme();

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    return (
      <>
        <List>
          <ListItem>
            <ListItemAvatar>
              <Avatar sx={{ bgcolor: NetworkStatusHighlight(data, theme) }}>
                <LanIcon />
              </Avatar>
            </ListItemAvatar>
            <ListItemText primary="Status" secondary={TransferNetworkStatus(data)} />
          </ListItem>
          <Divider variant="inset" component="li" />
          {
            isConnected(data) &&
            <>
              <ListItem>
                <ListItemAvatar>
                  <Avatar>IP</Avatar>
                </ListItemAvatar>
                <ListItemText primary="IP Address" secondary={data.local_ip} />
              </ListItem>
              <Divider variant="inset" component="li" />
              {data.local_ip_v6 && (
                <>
                  <ListItem>
                    <ListItemAvatar>
                      <Avatar>IP6</Avatar>
                    </ListItemAvatar>
                    <ListItemText primary="IPv6 Address" secondary={data.local_ip_v6} />
                  </ListItem>
                  <Divider variant="inset" component="li" />
                </>
                )}
              <ListItem>
                <ListItemAvatar>
                  <Avatar>
                    <DeviceHubIcon />
                  </Avatar>
                </ListItemAvatar>
                <ListItemText primary="MAC Address" secondary={data.mac_address} />
              </ListItem>
              <Divider variant="inset" component="li" />
              <ListItem>
                <ListItemAvatar>
                  <Avatar>#</Avatar>
                </ListItemAvatar>
                <ListItemText primary="Subnet Mask" secondary={data.subnet_mask} />
              </ListItem>
              <Divider variant="inset" component="li" />
              <ListItem>
                <ListItemAvatar>
                  <Avatar>
                    <SettingsInputComponentIcon />
                  </Avatar>
                </ListItemAvatar>
                <ListItemText primary="Gateway IP" secondary={data.gateway_ip || "none"} />
              </ListItem>
              <Divider variant="inset" component="li" />
              <ListItem>
                <ListItemAvatar>
                  <Avatar>
                    <DnsIcon />
                  </Avatar>
                </ListItemAvatar>
                <ListItemText primary="DNS Server IP" secondary={data.dns_ip} />
              </ListItem>
              <Divider variant="inset" component="li" />
              {data.full_duplex && (
                <>
                  <ListItem>
                    <ListItemAvatar>
                      <Avatar>
                        <CableIcon />
                      </Avatar>
                    </ListItemAvatar>
                    <ListItemText primary="Full Duplex" secondary={data.full_duplex} />
                  </ListItem>
                  <Divider variant="inset" component="li" />
                </>
                )}
              <ListItem>
                <ListItemAvatar>
                  <Avatar>
                    <SpeedIcon />
                  </Avatar>
                </ListItemAvatar>
                <ListItemText primary="Link Speed" secondary={`${data.link_speed} Mbps`} />
              </ListItem>
              <Divider variant="inset" component="li" />
            </>
          }
        </List>
        <ButtonRow pt={1}>
          <Button startIcon={<RefreshIcon />} variant="contained" color="secondary" onClick={loadData}>
            Refresh
          </Button>
        </ButtonRow>
      </>
    );
  };

  return (
    <SectionContent title='Network Status' titleGutter>
      {content()}
    </SectionContent>
  );

};

export default NetworkStatusForm;
