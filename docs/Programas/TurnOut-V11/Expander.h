#ifndef _Expander_h_
#define _Expander_h_

typedef class Expander* PtrExpander;

/**
 * Class representing a generic pin expander.
 */
class Expander
{
  protected:
    // A boolean indicating whether the expander has been initialized
    bool _initialized;

  public:
    /**
     * Base constructor for a generic expander.
     */
    Expander();

    /**
     * Auxiliary method to show the object ( equivalent to the toString() method of Java)
     */
    virtual void show() = 0;

    virtual bool begin() = 0;
};

#endif  // _Expander_h_
