# Json-Parser
A simple JSON Parser built in C++(98).

## Build
To use the parser in your project, you must first build the parser using the following command:

`make`

This will create a `libjsonparser.a` file in the repo directory. You can then link this library to your project and include the header file `JsonParser.hpp` in your source file.


## Usage
To use the parser first instantiate a `JsonParser` object with the JSON string as the argument and then call the `parse` method. The `parse` method then returns a `JsonValue` object which can be used to access the parsed JSON data.
If the JSON string is invalid, the `parse` method will throw an exception.

```cpp
try {

    JsonParser parser("sample.json");
    JsonValue json = parser.parse();

} catch (const std::exception& error) {
    std::cerr << error.what() << std::endl;
}
```

Then to access the parsed JSON data, you can use the `JsonValue` object as follows:
```cpp
JsonValue json = parser.parse();
for (objectIterator it = json.begin(); it != json.end(); ++it) {
    std::cout << it->first << " : " << it->second << std::endl;
}
```
This way you can iterate through the JSON object and access the main key-value pairs. There is also an `arrayIterator` to iterate through a JSON array.

To find out the type of the value, you can use the `getType()` method of the `JsonValue` object and then check with the `JsonValue::Type` enum.

```cpp
JsonValue json = parser.parse();
if (json.getType() == JsonValue::Type::OBJECT) {
    // Do something
}
```
## How to test
To run the included tests, use the following command:

`make test`

This will run the tests and print the results to the console.

To add more tests, add a new test JSON file in the `tests` directory.

## Caveats
- To iterate through the JSON object, you must use the `objectIterator` and to iterate through JSON arrays you use `arrayIterator` types defined in the `JsonValue` class. This is because the `JsonValue` class is a wrapper around a `std::map` and a `std::vector` and the iterators are defined as typedefs in the `JsonValue` class.
- The parser does not provide a way to identify the type of the array.
- The parser removes all whitespaces so if there are any whitespaces in a JSON string, they will be removed.
- The iteration through the JSON object is not in the order of the keys in the JSON string. This is because the `std::map` does not guarantee the order of the keys.

## License

## References
- [Making a Simple JSON Parser From Scratch](https://dev.to/uponthesky/c-making-a-simple-json-parser-from-scratch-250g)
