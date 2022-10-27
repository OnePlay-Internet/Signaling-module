
.\tools\protoc.exe --grpc_out=./webrtc/go .\protos\protobuf.proto --plugin=protoc-gen-grpc=./tools/protoc-gen-go-grpc.exe
.\tools\protoc.exe --go_out=./webrtc/go .\protos\protobuf.proto