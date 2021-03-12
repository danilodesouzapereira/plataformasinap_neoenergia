  SELECT DISTINCT dttData AS PerdaReguData FROM
    (
    SELECT dttData FROM PerdaReguPri
    UNION
    SELECT dttData FROM PerdaReguSec
    UNION
    SELECT dttData FROM PerdaReguEtd
    );
