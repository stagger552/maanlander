
import { createClient } from 'https://cdn.skypack.dev/@supabase/supabase-js';

const supabaseUrl = 'https://ptxsuqkrwagqzbhxektl.supabase.co'
const supabaseKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InB0eHN1cWtyd2FncXpiaHhla3RsIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MTM4NzY0ODUsImV4cCI6MjAyOTQ1MjQ4NX0.a0H_xT4bo0nsihpIO-rHjHvO0xmztjNIiKVwJSnwrdY"


const supabase = createClient(supabaseUrl, supabaseKey);




const { data, error } = await supabase.from('Data').select('*');


if (error) {
    console.log('Error fetching data:', error.message);
    
}

if (data) {
    createTable(data);
}
function createTable(data) {
    const container = document.getElementById('table-container');
    const table = document.createElement('table');
    table.setAttribute('class', 'data-table');

    // Create headers
    const headerRow = table.insertRow();
    if (data.length > 0) {
        Object.keys(data[0]).forEach(key => {
            const headerCell = document.createElement('th');
            headerCell.textContent = key;
            headerRow.appendChild(headerCell);
        });
    }

    // Populate data rows
    data.forEach(item => {
        const row = table.insertRow();
        Object.values(item).forEach(text => {
            const cell = row.insertCell();
            cell.textContent = text;
        });
    });

    container.appendChild(table);
}

fetchData();

