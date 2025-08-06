import dotenv from 'dotenv';
import express from 'express';
import { saveData } from './savedata.model.js'; // Mongoose model
import { ConnectMongoDB } from './MongoDBConnection.js';

dotenv.config();

const app = express();

app.get('/', (req, res) => {
  res.send('Hello World!');
});

// ✅ Dynamic save route
app.get('/save', async (req, res) => {
  const { fieldName, fieldVal } = req.query;

  if (!fieldName || !fieldVal) {
    return res.status(400).send('Missing fieldName or fieldVal in query');
  }

  try {
    await ConnectMongoDB();

    // Build object dynamically
    const dataToSave = { [fieldName]: fieldVal };

    const newData = new saveData(dataToSave);
    await newData.save();

    console.log(`✅ Saved { ${fieldName}: ${fieldVal} } to MongoDB`);
    res.send(`✅ Saved { ${fieldName}: ${fieldVal} } to MongoDB`);
  } catch (err) {
    console.error('❌ Error saving data:', err);
    res.status(500).send('Failed to save data.');
  }
});


// ✅ Start server
const port = process.env.PORT || 3000;
app.listen(port, () => {
  console.log(`🚀 Server running at http://localhost:${port}`);
});
