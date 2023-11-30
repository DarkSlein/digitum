import { FC, useContext } from 'react';

import { Avatar, Badge, List, ListItem, ListItemAvatar, ListItemIcon, ListItemText } from '@mui/material';
import LockOpenIcon from '@mui/icons-material/LockOpen';
import LockIcon from '@mui/icons-material/Lock';
import WifiIcon from '@mui/icons-material/Wifi';

import { WiFiEncryptionType, Network, NetworkList } from '../../types';

import { NetworkConnectionContext } from './NetworkConnectionContext';
import { MessageBox } from '../../components';

interface NetworkSelectorProps {
  networkList: NetworkList;
}

export const isNetworkOpen = ({ encryption_type }: Network) => encryption_type === WiFiEncryptionType.WIFI_AUTH_OPEN;

export const networkSecurityMode = ({ encryption_type }: Network) => {
  switch (encryption_type) {
    case WiFiEncryptionType.WIFI_AUTH_WEP:
    case WiFiEncryptionType.WIFI_AUTH_WEP_PSK:
      return "WEP";
    case WiFiEncryptionType.WIFI_AUTH_WEP2_PSK:
      return "WEP2";
    case WiFiEncryptionType.WIFI_AUTH_WPA_WPA2_PSK:
      return "WPA/WEP2";
    case WiFiEncryptionType.WIFI_AUTH_WPA2_ENTERPRISE:
      return "WEP2 Enterprise";
    case WiFiEncryptionType.WIFI_AUTH_OPEN:
      return "None";
    default:
      return "Unknown";
  }
};

const NetworkSelector: FC<NetworkSelectorProps> = ({ networkList }) => {
  const networkConnectionContext = useContext(NetworkConnectionContext);

  const renderNetwork = (network: Network) => {
    return (
      <ListItem key={network.bssid} button onClick={() => networkConnectionContext.selectNetwork(network)}>
        <ListItemAvatar>
          <Avatar>
            {isNetworkOpen(network) ? <LockOpenIcon /> : <LockIcon />}
          </Avatar>
        </ListItemAvatar>
        <ListItemText
          primary={network.ssid}
          secondary={"Security: " + networkSecurityMode(network) + ", Ch: " + network.channel}
        />
        <ListItemIcon>
          <Badge badgeContent={network.rssi + "db"}>
            <WifiIcon />
          </Badge>
        </ListItemIcon>
      </ListItem>
    );
  };

  if (networkList.networks.length === 0) {
    return (
      <MessageBox mt={2} mb={1} message="No WiFi networks found" level="info" />
    );
  }

  return (
    <List>
      {networkList.networks.map(renderNetwork)}
    </List>
  );

};

export default NetworkSelector;
