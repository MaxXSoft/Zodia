#ifndef ZODIA_DEFINE_PARENT_H_
#define ZODIA_DEFINE_PARENT_H_

// interface for parent containers
class ParentInterface {
 public:
  virtual ~ParentInterface() = default;
  // remove object from container
  virtual void RemoveObject(void *id) = 0;
};

using ParentPtr = ParentInterface *;

#endif  // ZODIA_DEFINE_PARENT_H_
