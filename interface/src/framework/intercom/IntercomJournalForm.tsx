import { FC } from "react";

import { Button, Divider, List, Paper, 
  Table, TableBody, TableCell, TableContainer, 
  TableHead, TableRow, useTheme } from "@mui/material";
import RefreshIcon from '@mui/icons-material/Refresh';

import * as IntercomApi from "../../api/intercom";
import { IntercomJournal } from "../../types";
import { ButtonRow, FormLoader, SectionContent } from "../../components";
import { useRest } from "../../utils";

const IntercomJournalForm: FC = () => {
  const { loadData, data, errorMessage } = useRest<IntercomJournal>({ read: IntercomApi.readIntercomJournal });

  const theme = useTheme();

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    return (
      <>
        <List>
        <TableContainer component={Paper}>
          <Table>
            <TableHead>
              <TableRow>
                <TableCell>Date</TableCell>
                <TableCell>Apartment</TableCell>
              </TableRow>
            </TableHead>
            <TableBody>
              {data.notes.map((note, index) => (
                <TableRow key={index}>
                  <TableCell>{note.createdAt}</TableCell>
                  <TableCell>{note.appartment}</TableCell>
                </TableRow>
              ))}
            </TableBody>
          </Table>
        </TableContainer>
          <Divider variant="inset" component="li" />
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
    <SectionContent title='Intercom Journal' titleGutter>
      {content()}
    </SectionContent>
  );

};

export default IntercomJournalForm;
