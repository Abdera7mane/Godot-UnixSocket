extends SceneTree

var socket_path: String = "/tmp/godot_unix_test.sock"
var data: String = "Hello Server"
var expected_response = "Hello Client"

func _init() -> void:
	var socket: StreamPeerUnix = StreamPeerUnix.new()
	var error: int

	print("CLIENT: Connected: ", socket.is_open())
	print("CLIENT: Opening...")
	error = socket.open(socket_path)
	print("CLIENT: Open error: ", error)
	print("CLIENT: Connected: ", socket.is_open())
	print("CLIENT: Path match: ", socket_path == socket.get_path())
	print("CLIENT: Sending: ", data)

	socket.put_data(data.to_ascii())
	var response_data: Array = socket.get_data(expected_response.length())
	var response: String = response_data[1].get_string_from_ascii()
	error = response_data[0]

	print("CLIENT: Expected response: ", expected_response)
	print("CLIENT: Response recieved: ", response)
	print("CLIENT: Response match: ", response == expected_response)
	print("CLIENT: Closing...")
	socket.close()
	print("CLIENT: Connected: ", socket.is_open())

	quit(0 if error == OK else 1)