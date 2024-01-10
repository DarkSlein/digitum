import { FC, useState } from "react";

import { Avatar, Box, Button, Divider, List, ListItem, ListItemAvatar, ListItemText, Theme, useTheme } from "@mui/material";
import PowerIcon from '@mui/icons-material/Power';
import PowerOffIcon from '@mui/icons-material/PowerOff';
import UpdateIcon from '@mui/icons-material/Update';
import DoorFrontIcon from '@mui/icons-material/DoorFront';
import MeetingRoomIcon from '@mui/icons-material/MeetingRoom';
import QuestionMarkIcon from '@mui/icons-material/QuestionMark';
import NotificationsIcon from '@mui/icons-material/Notifications';
import RefreshIcon from '@mui/icons-material/Refresh';
import { useSnackbar } from "notistack";

import * as IntercomApi from "../../api/intercom";
import { DoorStatus, IntercomConnectionStatus, IntercomStatus, SwitchDoorType } from "../../types";
import { ButtonRow, FormLoader, SectionContent } from "../../components";
import { extractErrorMessage, useRest } from "../../utils";

export const intercomStatusHighlight = ({ status }: IntercomStatus, theme: Theme) => {
  switch (status) {
    case IntercomConnectionStatus.CONNECTED:
      return theme.palette.success.main;
    case IntercomConnectionStatus.NOT_CONNECTED:
      return theme.palette.info.main;
    case IntercomConnectionStatus.RECEIVING_DATA:
      return theme.palette.success.main;
    default:
      return theme.palette.warning.main;
  }
};

export const intercomStatusIcon = (status: IntercomConnectionStatus) => {
  switch (status) {
    case IntercomConnectionStatus.CONNECTED:
      return <PowerIcon />;
    case IntercomConnectionStatus.NOT_CONNECTED:
      return <PowerOffIcon />;
    case IntercomConnectionStatus.RECEIVING_DATA:
      return <UpdateIcon />;
    default:
      return <QuestionMarkIcon />;
  }
};

export const doorStatusIcon = (status: DoorStatus) => {
  switch (status) {
    case DoorStatus.CLOSED:
      return <DoorFrontIcon />;
    case DoorStatus.OPENED:
      return <MeetingRoomIcon />;
    default:
      return <QuestionMarkIcon />;
  }
};

export const intercomStatus = ({ status }: IntercomStatus) => {
  switch (status) {
    case IntercomConnectionStatus.CONNECTED:
      return "Active";
    case IntercomConnectionStatus.NOT_CONNECTED:
      return "Inactive";
    case IntercomConnectionStatus.RECEIVING_DATA:
      return "Receiving data";
    default:
      return "Unknown";
  }
};

export const doorStatus = (status: DoorStatus) => {
  switch (status) {
    case DoorStatus.OPENED:
      return "Opened";
    case DoorStatus.CLOSED:
      return "Closed";
    default:
      return "Unknown";
  }
};

export const openDoorApi = async () => {
  await IntercomApi.switchDoor({
    type: SwitchDoorType.JOGGING,
    status: DoorStatus.OPENED,
    time: 1
  })
};

const IntercomStatusForm: FC = () => {
  const { loadData, data, errorMessage } = useRest<IntercomStatus>({ read: IntercomApi.readIntercomStatus });
  const { enqueueSnackbar } = useSnackbar();
  const [confirmRestart, setConfirmRestart] = useState<boolean>(false);
  const [processing, setProcessing] = useState<boolean>(false);

  const theme = useTheme();

  const openDoor = async () => {
    setProcessing(true);
    try {
      await openDoorApi();
      enqueueSnackbar("Door is opened", { variant: 'success' });
    } catch (error: any) {
      enqueueSnackbar(extractErrorMessage(error, 'Problem restarting device'), { variant: 'error' });
    } finally {
      setConfirmRestart(false);
      setProcessing(false);
    }
  };

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    return (
      <>
        <List>
          <ListItem>
            <ListItemAvatar>
              <Avatar sx={{ bgcolor: intercomStatusHighlight(data, theme) }}>
              {intercomStatusIcon(data.status)}
              </Avatar>
            </ListItemAvatar>
            <ListItemText primary="Status" secondary={intercomStatus(data)} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <ListItemAvatar>
              <Avatar>
                <NotificationsIcon />
              </Avatar>
            </ListItemAvatar>
            <ListItemText primary="Last Called Number" secondary={data.lastCalledNumber} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <ListItemAvatar>
              <Avatar>
                {doorStatusIcon(data.doorStatus)}
              </Avatar>
            </ListItemAvatar>
            <ListItemText primary="Door Status" secondary={doorStatus(data.doorStatus)} />
          </ListItem>
          <Divider variant="inset" component="li" />
        </List>

        <Box display="flex" flexWrap="wrap">
          <Box flexGrow={1}>
            <ButtonRow mt={1}>
              <Button startIcon={<RefreshIcon />} variant="contained" color="secondary" onClick={loadData}>
                Refresh
              </Button>
            </ButtonRow>
          </Box>
          <Box flexWrap="nowrap" whiteSpace="nowrap">
            <ButtonRow mt={1}>
              <Button startIcon={<MeetingRoomIcon />} variant="contained" color="info" onClick={openDoor}>
                Open door
              </Button>
            </ButtonRow>
          </Box>
        </Box>
      </>
    );
  };

  return (
    <SectionContent title='Intercom Status' titleGutter>
      {content()}
    </SectionContent>
  );

};

export default IntercomStatusForm;
