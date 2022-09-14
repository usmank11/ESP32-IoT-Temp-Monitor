import React from 'react';
import './App.css';
import { db } from "./firebase";
import { ref, onValue } from "firebase/database";
import { useState, useEffect } from "react";
import Card from '@mui/material/Card';
import CardContent from '@mui/material/CardContent';


function App() {

  const temp = ref(db, 'sensor/temperature');
  const humid = ref(db, 'sensor/humidity');

  function tempUpdate() {
    const [value, setValue] = useState();
    useEffect(() => {
      onValue(temp, (snapshot) => {
        const data = snapshot.val();
        setValue(data);
      })
    });
    return value;
  }


  function humidUpdate() {
    const [value, setValue] = useState();
    useEffect(() => {
      onValue(humid, (snapshot) => {
        const data = snapshot.val();
        setValue(data);
        })
      });
    return value;
  }

  const tempVal = tempUpdate();
  console.log(tempVal);

  const humidVal = humidUpdate();
  console.log(humidVal);

  function BasicCard() {
    return (
      <Card sx={{ maxWidth: 300 }}>
        <CardContent>
          <div className='Text'>Temperature: {tempVal}°C</div>
          <div className='Text'>Humidity: {humidVal}%</div>
        </CardContent>
      </Card>
    );
  }
  
  return (
    <>
      <header className='Header'>
      <h1>Temperature Sensor</h1>
      </header>
      <center>
        <BasicCard className='center'>
        </BasicCard>
      </center>
    </>
  );
}

export default App;
