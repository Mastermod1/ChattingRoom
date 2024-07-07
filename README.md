# Chatting Room

## How to run

```bash
bazel run //src:main
```
## Testing

To run all the tests use this command
```bash
bazel test //test/...
```

## To do
- extract server to its own class so that the server can be hosted independently
- scrolling incoming messages
- add commands like /disconnect
- add current user list
- add dropdown list with recently used ip address
- handle user disconnection on the server


## License

[MIT](https://choosealicense.com/licenses/mit/)
