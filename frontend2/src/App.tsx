import { useState } from 'react';

function App() {
  const [responseMessage, setResponseMessage] = useState('');
  
  const handleButtonClick = () => {
    fetch('http://%API_URL%/api/v1/door/open')
      .then(response => {
        if (response.status === 200) {
          setResponseMessage('Successful');
        } else {
          setResponseMessage('Request failed');
        }
      })
      .catch(() => {
        setResponseMessage('Request failed');
      });
  };

  return (
    <div className="centered">
      <div className="wrapper">
        <h1>
          <span>Door</span>
        </h1>
          <button onClick={handleButtonClick}>Open door</button>
          <p>{responseMessage}</p>
      </div>
    </div>
  );
}

export default App;
