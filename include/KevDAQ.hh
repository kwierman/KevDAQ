#ifndef KevDAQ_hh_
#define KevDAQ_hh_

namespace kevdaq
{
  
  class KevDAQClass{
    int fMyInt;
  protected:
    void setInt(int const&);
  public:
    KevDAQClass(int const&);
    ~KevDAQClass();
    int getInt();
  };

}

#endif //KevDAQ_hh_