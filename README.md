# BSDL Parser
This is a bsdl parser which based on IEEE 1149.1 Boundary-Scan Description Language (BSDL) files.

## Objective
This project is to parse the BSDL file and store the data into memory. It will return the data that user need to know by calling specific function. This will help user to save their time by not searching for the data from BSDL file line by line.

## File to parse
- BSDL file (.bsdl or .bsd)

## Environment
- Window 10
- C Language

## Current feature that it can handle: 
- Component Name
- Generic Parameter
- Logical Port Description
- Standard Use Statement
- Component Conformance Statement
- Device Package Pin Mappings
- Scan Port Identification
- Instruction Length
- Boundary Length
- Boundary Register

## Example Output When An Error Is Detected
Input:

![Input data](https://trello-attachments.s3.amazonaws.com/5cee32732804e15c00a9f155/5d663c8ea583411ec782886d/5c423ea35ae1c9043d48f40358619b9a/input_value.PNG)

Output:

![Output](https://trello-attachments.s3.amazonaws.com/5cee32732804e15c00a9f155/5d663c8ea583411ec782886d/e1527f21472154b2cf2f0b090fb92b02/result_output.PNG)

## Future Goal
- Complete the BSDL parser 
- Follow all the rules for each description
- Add feature for calling to get the data. 
Example: getPackageName(Bsinfo) => return packageName
![future implementation](https://trello-attachments.s3.amazonaws.com/5cee32732804e15c00a9f155/5d663c8ea583411ec782886d/cea69b777eaea48539dec0a64270a3b3/future_implement.PNG)


## Reference
IEEE 1149.1-2013 - IEEE Standard for Test Access Port and Boundary-Scan Architecture
