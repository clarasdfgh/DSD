# Charla 06-05: distributed systems middleware models and architecture

- Middleware: underlying tech for distributed systems, some of which overlap
- Definition: a DS is a system whose components (located in computers connectec to a network) only communicate via exchange of messages. Essential characteristics:
  - Concurrency
    - Cooperation, common goal
  - No global clock
    - No global components, they need to interact and compaginarse only via exchange of messages
  - Independent component failures
    - Delays, message loss, attacks
- Design challenges:
  - Complexity factors:
    - Heterogeneity: you cannot assume that they are homogeneous, they cooperate or compete for resources
    - uuuuuu
    - Security:
    - Concurrency:
    - Transparency: 8 meanings, depends on where applied to. 
      - We don't need to know where it's located, we just want location independently
    - Scalability: the ability to provide good quality no matter how much demand there is
    - Failures: the possibility to control the system even in front of failure. Decentralization. SPOF, single point of failure. We want scalability, but also uuuu
- Middleware: software layer between application services and operating system/platform
  - Why? 
    - Car example, you want a simple car. Do you expect the best performance? No, that would be a F1 car. The difference between a regular car and a F1 car is low level, its engine is better engineered.

- Low level: best performance, in exchange of lower productivity (more difficult to write). Also less complexity.
  - To model the best software systems we need to work lower level 
  - The goal of a middleware is to obtain the complexity (that we get with abstraction) of higher levels with the performance of lower levels.
- Middleware had its boom about 20 years ago. We use it for application integration (its known as glue tech)
  - We wont talk about transactional middleware, which is very specific to finance
    - (seems like we WILL be talking about this since someone asked) Supermarket. Two phase commit: first, decrease an amount from your banking account. Second, increase that same amount to the supermarket's account. Those are atomic operations, we need them both to happen or none at all. ACID properties. We cannot ever assume that we well be using the same DBMS, bank, etc... that's why we need the middleware layer on top of the DBSM. 
- Middlware:
  - Communication oriented
    - MOM - Message Oriented Middleware
  - Data oriented
    - RDA
    - TP
    - TS
  - Computation oriented
    - RPC - Remote Procedure Call
    - DOM
    - CM
    - SOA

- RPC:
  - Why? The goal is to invoke a program residing in a different node (client-server). What does this accomplish?
    - All traditional programmers would be able to become distributed programmers. Before RPC the programmer would have to first learn about protocols, when we automatize this process it's not necessary anymore.
  - The invoker becomes a client, the provider becomes a server
  - How does RPC work? You hace a prototype, an specification of the remote procedure. When you compile with the RPC tech you get two stubs, the client and the server. You compile the client, then the server, then link them. Linking is not straightforward bc they're in different nodes. The stub of the caller does not change (comparing to a single node app), but the server must receive a message from it that invokes the procedure. This invocation is local, not remote: the remote part is the message .
  - The parameters in the client stub are sent to the other node. These parameters must be prepared in case of different languages etc.. this is called marshalling, and it's done in the server side.
  - Semantics: we might have failures. Imagine we don't get a return: has it been executed? we don't really know. We need protocols TCP or UDP
    - UDP: uuuu
    - TCP: uuuu
    - Idempotent

- MOM:
  - Uses queues. Indirect, asynchronous communication
  - Allows one to many, and many to many
  - Producer-consumer. But messages may be typed, so the consumers have to be picky about what messages they consume, that's what the MOM's for.
  - In case of app failure, the middleware must be able to store and later (when it's fixed) forward the messages. 
  - It's a publish-subscribe model: it allows the publisher to not know the receiver, rather the receiver just decides to subscribe to the publisher.
    - Different from notification
      - Pull/Push system
      - Similar to the observer pattern
    - Decoupling: three types:
      - Spatial: neither the pub-sub know their respective identities
      - Temporal: pub may produce events even when the sub is not connected. Sub may receive events even when pub is disconnected
      - Synchronization: pub uuuu
    - Used in the EU air traffic control due to different laws on aviation in different countries.
    - JMS is a Java implementation for MOM, but it obviously does not provide language transparency. Therefore we might need a middleware for other languages, such as Apache activeMQ

---descanso---

- Blockchain: could it improve banking systems? Yes and no, mostly no. In theory yes, but blockchain depends on no central entity: that's the opposite of a bank.
- Tuple space model: based on one of the first middleware systems
  - Tuples are records, data structures. Applications communicate through the abstraction of a virtual shared space.
  - Access by type and content, not address.
  - Implemented in JavaSpace: the tuple space becomes an object space. Objects have a lease, the time they're available to be accessed.
  - Master-Worker model.

- Distributed Objects: can we invoke methods remotely? Why do we need to do that? Why is object orientation good?
  - An object is an encapsulation of a concrete data structure into operations that are the only thing that can modify that data.
  - In OOP we hide the data structure and the algorithm of out methods (two things). That's why OOP is better than abstract uuuu. Higher level of abstraction.
  - OOP and distributed systems don't really mix. We do client-server because it's a good fistributed computing model. The server is static, it's there; and the client starts the interaction: it's the only active identity in this operation. 
    - An object would be a passive entity, reactive. It would be on the server side, but it resides in the client side. What do we do?
    - We remotely invoke a method: we again have a stub on the client side, and an skeleton on the server side that communicate via message bus, ORB or RMI.
    - We need to know what the object is, so we use a catalog. But we don't know if the object requires internally other objects, the interface only gives us what's offered, not what's required: that takes us to the component model, since the catalog was too small.
      - The component model let's us know what it gives, and what it requires
      - It allows reusability
        - OOP Is ok but it only provides reusability from source code
      - Components model consists of not only classes but executables

- Service Oriented Architectures SOAS
  - It's not an object, but a service: loosely coupled, protocol independent, standard based distributed programming
  - It's closer to business, more powerful than objects
  - Provider, Consumer, Registry: provider offers a service, adds it to the registry, consumer uses it, there's constance of it in the registry.

- Web Services, standards: uuuuuu
- Micro Services Architecture: next step in distributed systems. Same concept of SOA, but every microservice encapsulates a business functionality. It doesn't mean that they're small. Single functionality.
  - Netflix example: the title of a movie, the cast, the like/dislike button.... Each is provided by a different microservice. If a single microservice is down, I as a customer probably won't realize, it creates a great experience. 
  - It's all about failure toleration. 
  - It also allows to change functionality almost seamlessly: they don't need the test and run model, it's all done in production. Because chances are almost no one will realize.

