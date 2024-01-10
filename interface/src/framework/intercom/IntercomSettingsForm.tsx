import { FC, useState } from 'react';
import { ValidateFieldsError } from 'async-validator';

import { Button, Checkbox, MenuItem } from '@mui/material';
import SaveIcon from '@mui/icons-material/Save';

import * as IntercomApi from "../../api/intercom";
import { IntercomSettings } from '../../types';
import { ButtonRow, FormLoader, SectionContent, ValidatedTextField } from '../../components';
import { createIntercomSettingsValidator, validate } from '../../validators';
import { updateValue, useRest } from '../../utils';

const IntercomSettingsForm: FC = () => {
  const [fieldErrors, setFieldErrors] = useState<ValidateFieldsError>();
  const {
    loadData, saving, data, setData, saveData, errorMessage
  } = useRest<IntercomSettings>({ read: IntercomApi.readIntercomSettings, update: IntercomApi.updateIntercomSettings });

  const updateFormValue = updateValue(setData);

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    const validateAndSubmit = async () => {
      try {
        setFieldErrors(undefined);
        await validate(createIntercomSettingsValidator(data), data);
        saveData();
      } catch (errors: any) {
        setFieldErrors(errors);
      }
    };

    return (
      <>
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="kmnModel"
          label="KMN Model"
          value={data.kmnModel}
          fullWidth
          select
          variant="outlined"
          onChange={updateFormValue}
          margin="normal"
        >
          {
            data.kmnModelList.map((item) => (
              <MenuItem key={item} value={item}>
                {item}
              </MenuItem>
            ))
          }
        </ValidatedTextField>
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="firstApartment"
          label="First Appartment"
          fullWidth
          variant="outlined"
          value={data.firstApartment}
          onChange={updateFormValue}
          margin="normal"
        />
        <ButtonRow mt={1}>
          <Button startIcon={<SaveIcon />} disabled={saving} variant="contained" color="primary" type="submit" onClick={validateAndSubmit}>
            Save
          </Button>
        </ButtonRow>
      </>
    );
  };

  return (
    <SectionContent title='Intercom Settings' titleGutter>
      {content()}
    </SectionContent>
  );
};

export default IntercomSettingsForm;
