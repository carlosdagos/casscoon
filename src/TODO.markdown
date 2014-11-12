TODO
====

- Work on global install script
- Always update the README file
- Export more methods for Connection class (allow for credentials)
- Add batches
- Add parallel queries
- Export data types
- Add connection pools
	- Research connection pools
- Maybe we don't want to do automatic binding
	- On that note, maybe we want to have automatic binding and manual
		- Went for manual, automatic makes things harder

BUGS
====
- String encoding on some queries
    - Sometimes with weird appendages
    - Some characters don't work cross-platform

PERTINENT TESTS
===============
- Connections
	- Connection pooling
	- Basic connections
	- Connections with credentials
	- Connection policies
- Queries
	- With scalar types
	- With complex types
- Consistency settings
	- With replication lower than number of nodes
	- With replication higher than number of nodes (shall produce Exceptions)
- Batches
	- Basic batches
	- Batches with consistency settings
- Parallel queries
	- Async design
	- Callbacks?
- Benchmarks
	- Threaded
	- CLI
	- CGI