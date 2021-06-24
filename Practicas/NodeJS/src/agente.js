import '/node_modules/jquery/dist/jquery.min.js';
import '/socket.io/socket.io.js';

const serviceURL = document.URL.substring(0, document.URL.lastIndexOf("/"));
const socket     = io.connect(serviceURL);

function initLum () {
	const lum = $('#lum');

		lum.on('input', function (){
			socket.emit('sensor-status', {name: 'Luminosity', value: Number(this.value)});
		});

	$('#valor_lum').html(lum.attr('value'));
}

function initTemp ()
{
	const tem = $('#tem');

		tem.on('input', function () {
			socket.emit('sensor-status', {name: 'Temperature', value: Number(this.value)});
		});

	$('#valor_tem').html(tem.attr('value'));
}


function initSocket () {
	socket.on('connect', () =>
	{
		socket.emit('connection-status', 'env');
	});

	socket.on('update-sensors', (event) =>
	{
		if (event.name === "Luminosity")
			$('#valor_lum').html(event.value);
		else if (event.name === "Temperature")
			$('#valor_tem').html(event.value);
	});

	socket.on('update-systems', (event) =>
	{
		if (event.name === "AC")
		{
			const status = $('#estado_aire');

			if (event.value === "on")
				status.text("encendido");
			else
				status.text("apagado");
		}
		else if (event.name === "Blinds")
		{
			const status = $('#estado_persiana');

			if (event.value === "open")
				status.text("abiertas");
			else
				status.text("cerradas");
		}

	});
}


$(() =>
{
	initLum();
	initTemp();
	initSocket();
});
