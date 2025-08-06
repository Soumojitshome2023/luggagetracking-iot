import mongoose, { Schema } from "mongoose";

// Allows any key-value pairs (flexible schema)
const saveDataSchema = new Schema({}, { strict: false, timestamps: true });

export const saveData = mongoose.models.saveDatas || mongoose.model("saveDatas", saveDataSchema);
